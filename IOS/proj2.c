/* 
 * Reseni IOS: DU2, 30.4.2018
 * Autor: Adam Sedláček, FIT
 * Email: xsedla1e@stud.fit.vutbr.cz
 * Prelozeno: gcc 6.3.0, Debian 9.3
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>

#define BASE_DEC (10)
#define ARG_NUMBER (4)
#define ARG_R (0)
#define ARG_C (1)
#define ARG_ART (2)
#define ARG_ABT (3)
#define F_OUT "proj2.out"

#define SHARED_CREATE(ptr) { (ptr) = mmap(NULL, sizeof(*(ptr)), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); }
#define SHARED_DESTROY(ptr) { munmap((ptr), sizeof(ptr)); }
#define MY_SEM_OPEN(ptr, sem_name, val) { (ptr) = sem_open((sem_name), O_CREAT | O_EXCL, 0666, (val)); }
// #define my_error(msg) { fprintf(stderr, msg, strerror(errno)); }

/*******************************************************************************

	POKUD NEKDO OPRAVDU CTE TENTO KOD, PROSIM OZVETE SE MI NA E-MAIL, DEKUJI

*******************************************************************************/

FILE *fp = NULL;

int *line_cnt = NULL;
int *ppl_waiting = NULL;
int *passengers = NULL;
int *ppl_on_way = NULL;

sem_t *enter_sem = NULL;
sem_t *get_on_sem = NULL;
sem_t *finished_sem = NULL;
sem_t *check_finished_sem = NULL;
sem_t *counter_sem = NULL;

inline void my_sleep(int sleep_time) {
	if (sleep_time != 0) {
		usleep(rand() % (sleep_time * 1000 + 1));
	}
}

inline void my_error(const char *msg) {
	fprintf(stderr, msg, strerror(errno));
}
bool init() {
	bool sem_fail = false;
	bool shared_fail = false;
	fp = fopen(F_OUT, "w");

	if (fp == NULL) {
		my_error("Nelze vytvorit soubor, nejspise limitace prav\n");
		exit(EXIT_FAILURE);
	}

	setbuf(fp, NULL);

	SHARED_CREATE(line_cnt);
	SHARED_CREATE(ppl_waiting);
	SHARED_CREATE(passengers);
	SHARED_CREATE(ppl_on_way);

	if (line_cnt == MAP_FAILED) shared_fail = true;
	if (ppl_waiting == MAP_FAILED) shared_fail = true; 
	if (passengers == MAP_FAILED) shared_fail = true;
	if (ppl_on_way == MAP_FAILED) shared_fail = true;

	if (shared_fail) {
		my_error("Namapovani sdilene promenne se nezdarilo\n");
		return 1;
	}

	*line_cnt = 1;
	*ppl_waiting = 0;
	*passengers = 0;
	*ppl_on_way = 0;

	MY_SEM_OPEN(enter_sem, "/xsedla1e_stop", 1);
	MY_SEM_OPEN(get_on_sem, "/xsedla1e_board", 0);
	MY_SEM_OPEN(finished_sem, "/xsedla1e_rider", 0);
	MY_SEM_OPEN(check_finished_sem, "/xsedla1e_control", 0);
	MY_SEM_OPEN(counter_sem, "/xsedla1e_counter", 1);

	if (enter_sem == SEM_FAILED) sem_fail = true;
	if (get_on_sem == SEM_FAILED) sem_fail = true;
	if (finished_sem == SEM_FAILED) sem_fail = true;
	if (check_finished_sem == SEM_FAILED) sem_fail = true;
	if (counter_sem == SEM_FAILED) sem_fail = true;

	if (sem_fail) {
		my_error("Nepovedlo se otevrit semafor\n");
		return 1;
	}

	return 0;
}

void cleanup() {
	if (fp != NULL) {
		fclose(fp);
	}
	
	SHARED_DESTROY(line_cnt);
	SHARED_DESTROY(ppl_waiting);
	SHARED_DESTROY(passengers);
	SHARED_DESTROY(ppl_on_way);

	sem_close(enter_sem);
	sem_close(get_on_sem);
	sem_close(finished_sem);
	sem_close(check_finished_sem);
	sem_close(counter_sem);

	sem_unlink("/xsedla1e_stop");
	sem_unlink("/xsedla1e_board");
	sem_unlink("/xsedla1e_rider");
	sem_unlink("/xsedla1e_control");
	sem_unlink("/xsedla1e_counter");
}

bool is_number(const char *arg, int *give_n) {
	char *endptr = NULL;
	*give_n = (int)strtol(arg, &endptr, BASE_DEC);
	return !(*endptr == '\0');
}

void verify_par(int *arr_par, int argc, char *argv[]) {
	char par_name[ARG_NUMBER][ARG_NUMBER] = {"R", "C", "ART", "ABT"};
	if (argc == 5) {
		for (int i = 0; i < ARG_NUMBER; i++) {
			if (is_number(argv[i + 1], &arr_par[i])) {
				fprintf(stderr, "Chyba argumentu: %s zadano: %s, ocekavano cislo\n",
					par_name[i], argv[i + 1]);
				exit(EXIT_FAILURE);
			}
		}
	} else {
		my_error("Chyba poctu parametru\n");
		exit(EXIT_FAILURE);
	}
}

void verify_range(int *arr_par) {
	if ((arr_par[ARG_R] <= 0) || (arr_par[ARG_C] <= 0)) {
		fprintf(stderr, "Neplatne (A, C) > 0\n");
		exit(EXIT_FAILURE);
	}

	if ((arr_par[ARG_ART] < 0 || arr_par[ARG_ART] > 1000) ||
			(arr_par[ARG_ABT] < 0 || arr_par[ARG_ABT] > 1000)) {
		fprintf(stderr, "Rozsah pro 1000 >= (ART, ABT) >= 0\n");
		exit(EXIT_FAILURE);
	}
}

void proc_rider(int rider_num) {
	sem_wait(counter_sem);
    fprintf(fp, "%d : RID %d : start\n", *line_cnt, rider_num);
    (*line_cnt)++;
    sem_post(counter_sem);

    sem_wait(enter_sem);
    sem_wait(counter_sem);
    (*ppl_waiting)++;
    fprintf(fp, "%d : RID %d : enter: %d\n", *line_cnt, rider_num, *ppl_waiting);
    (*line_cnt)++;
    sem_post(counter_sem);
    sem_post(enter_sem);

    sem_wait(get_on_sem);
    sem_wait(counter_sem);
    (*ppl_waiting)--;
    (*ppl_on_way)++;
    fprintf(fp, "%d : RID %d : boarding\n", *line_cnt, rider_num);
    (*line_cnt)++;
    sem_post(counter_sem);
    sem_post(check_finished_sem);

    sem_wait(finished_sem);
    sem_wait(counter_sem);
    (*ppl_on_way)--;
    (*passengers)++;
    fprintf(fp, "%d : RID %d : finish\n", *line_cnt, rider_num);
    (*line_cnt)++;
    sem_post(counter_sem);
    sem_post(check_finished_sem);

	exit(EXIT_SUCCESS);
}

void proc_bus(int cap, int delay, int riders) {
	sem_wait(counter_sem);
    fprintf(fp, "%d : BUS : start\n", *line_cnt);
    (*line_cnt)++;
    sem_post(counter_sem);

    while(riders > *passengers) {
        sem_wait(enter_sem);
        sem_wait(counter_sem);
        fprintf(fp, "%d : BUS : arrival\n", *line_cnt);
        (*line_cnt)++;
        sem_post(counter_sem);

        if(*ppl_waiting != 0) {
            sem_wait(counter_sem);
            fprintf(fp, "%d : BUS : start boarding: %d\n", *line_cnt, *ppl_waiting);
            (*line_cnt)++;
            sem_post(counter_sem);

            while(*ppl_on_way < cap && *ppl_waiting > 0) {
                sem_post(get_on_sem);
                sem_wait(check_finished_sem);
            }

            sem_wait(counter_sem);
            fprintf(fp, "%d : BUS : end boarding: %d\n", *line_cnt, *ppl_waiting);
            (*line_cnt)++;
            sem_post(counter_sem);
        }

        sem_wait(counter_sem);
        fprintf(fp, "%d : BUS : depart\n", *line_cnt);
        (*line_cnt)++;
        sem_post(counter_sem);
        sem_post(enter_sem);

        my_sleep(delay);
        sem_wait(counter_sem);
        fprintf(fp, "%d : BUS : end\n", *line_cnt);
        (*line_cnt)++;
        sem_post(counter_sem);

        while(*ppl_on_way > 0) {
            sem_post(finished_sem);
            sem_wait(check_finished_sem);
        }
    }

    sem_wait(counter_sem);
    fprintf(fp, "%d : BUS : finish\n", *line_cnt);
    (*line_cnt)++;
    sem_post(counter_sem);

	exit(EXIT_SUCCESS);	
}

void spawn_riders(int riders_num, int delay) {

	for (int i = 1; i <= riders_num; i++) {
		pid_t r_id = fork();

		if (r_id < 0) {
			my_error("Nepovedl se fork rider\n");
			exit(EXIT_FAILURE);
		}

		if (r_id == 0) {
			proc_rider(i);
		}

		my_sleep(delay);
	}

	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
	int arr_par[ARG_NUMBER] = { 0 };

	setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    srand(time(NULL));

	verify_par(arr_par, argc, argv);
	verify_range(arr_par);

	if(init()) {
		cleanup();
		my_error("Spuste program znovu, semafor nebyl radne unlinknut\n");
		exit(EXIT_FAILURE);
	}

	pid_t bus = fork();

	if (bus < 0) {
        my_error("Nepovedl se fork pro proces BUS\n"); 
        exit(EXIT_FAILURE);
    } else if (bus == 0) {
    	proc_bus(arr_par[ARG_C], arr_par[ARG_ABT], arr_par[ARG_R]);
    } else {
    	pid_t gen_riders = fork();

    	if (gen_riders < 0) {
	        my_error("Nepovedl se fork pro proces na generování riders\n"); 
	        exit(EXIT_FAILURE);
    	} else if (gen_riders == 0) {
       		spawn_riders(arr_par[ARG_R], arr_par[ARG_ART]);
    	} else {
    		int returnStatus;    
    		
    		waitpid(gen_riders, &returnStatus, 0);

    		if (returnStatus == 1) {
		       my_error("Nejaky z procesu neskoncil uspesne\n");    
		    }
    	}
    }

    cleanup();
	return EXIT_SUCCESS;
} 