<?php declare(strict_types = 1);

namespace App\Module\Reservation\Factory;

use App\Module\Core\Datagrid\DataGridFactory;
use App\Module\Core\Security\User;
use App\Module\Reservation\Entity\Reservation;
use App\Module\Reservation\Enum\ReservationStateType;
use App\Module\Reservation\Repository\ReservationRepository;
use App\Module\User\Entity\User as UserEntity;
use App\Module\User\Service\AccessGuard;
use Doctrine\ORM\QueryBuilder;
use Ublaboo\DataGrid\DataGrid;
use Ublaboo\DataGrid\DataSource\DoctrineDataSource;

/**
 */
class ReservationListFactory
{
    public const
        TITLE = 'title',
        EMAIL = 'email',
        TYPE = 'type',
        DATE_TIME = 'dateTime',
        HALL = 'hall',
        FORMATTED_PRICE = 'formattedPrice',
        STATE_AS_STRING = 'stateAsString';

    /** @var DataGridFactory */
    private $dataGridFactory;

    /** @var ReservationRepository */
    private $reservationRepository;

    /** @var User */
    private $user;

    public function __construct(DataGridFactory $dataGridFactory, ReservationRepository $reservationRepository, User $user)
    {
        $this->dataGridFactory = $dataGridFactory;
        $this->reservationRepository = $reservationRepository;
        $this->user = $user;
    }

    public function create(?UserEntity $userEntity = null, bool $showActions = false): DataGrid
    {
        $grid = $this->dataGridFactory->create();
        $grid->setPagination(false);
        $grid->setDataSource(new DoctrineDataSource($this->getQueryBuilder($userEntity), 'r.id'));
        $grid->addColumnLink(self::TITLE, 'Název', 'detail!', 'event.content.title');
        $grid->addColumnDateTime(self::DATE_TIME, 'Datum a čas', 'event.dateTime');
        $grid->addColumnText(self::EMAIL, 'E-mail');
        $grid->addColumnText(self::TYPE, 'Typ', 'event.content.type.title');
        $grid->addColumnText(self::HALL, 'Sál', 'event.hall.title');
        $grid->addColumnText(self::FORMATTED_PRICE, 'Cena');
        $grid->addColumnText(self::STATE_AS_STRING, 'Stav');

        $this->conditionallyAppendCashierActions($grid, $showActions);

        return $grid;
    }

    private function conditionallyAppendCashierActions(DataGrid $grid, bool $showActions): void
    {
        if ($showActions && AccessGuard::hasCashierAccess($this->user->getEntity())) {
            $grid->addAction('pay!', 'Zaplatit')
                ->setClass(static function(Reservation $reservation) {
                    return $reservation->getState() === ReservationStateType::PAID() ? 'hidden' : '';
                });
            $grid->addAction('remove!', 'Smazat');
        }
    }

    private function getQueryBuilder(?UserEntity $user = null): QueryBuilder
    {
        $qb = $this->reservationRepository->getQueryBuilder();

        if ($user) {
            $qb->where('r.user = :user')
                ->setParameter('user', $user);
        }

        return $qb;
    }
}
