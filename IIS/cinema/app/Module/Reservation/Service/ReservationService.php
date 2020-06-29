<?php declare(strict_types = 1);

namespace App\Module\Reservation\Service;

use App\Module\Content\Entity\Event;
use App\Module\Core\Security\User;
use App\Module\Reservation\Container\ReservationContactInfoContainer\ReservationContactInfoData;
use App\Module\Reservation\Container\ReservationSeatContainer\ReservationSeatData;
use App\Module\Reservation\Entity\Reservation;
use App\Module\Reservation\Enum\ReservationStateType;
use Doctrine\ORM\EntityManagerInterface;

/**
 */
class ReservationService
{
    /** @var EntityManagerInterface */
    private $entityManager;

    /** @var User */
    private $user;

    /** @var TicketService */
    private $ticketService;

    public function __construct(EntityManagerInterface $entityManager, TicketService $ticketService, User $user)
    {
        $this->entityManager = $entityManager;
        $this->ticketService = $ticketService;
        $this->user = $user;
    }

    public function changeReservationState(Reservation $reservation, ReservationStateType $stateType): void
    {
        $reservation->setState($stateType);
        $this->entityManager->persist($reservation);
        $this->entityManager->flush();
    }

    public function removeReservation(Reservation $reservation): void
    {
        $this->entityManager->remove($reservation);
        $this->entityManager->flush();
    }

    public function saveNewReservation(Event $event, ReservationContactInfoData $contactInfoData, ReservationSeatData $seatData): Reservation
    {
        $reservation = new Reservation(
            $contactInfoData->getFirstName(),
            $contactInfoData->getLastName(),
            $contactInfoData->getEmail(),
            $contactInfoData->getPhone(),
            $event,
            ReservationStateType::UNPAID(),
            $contactInfoData->getUser()
        );
        $this->entityManager->persist($reservation);
        $this->entityManager->flush();

        $this->ticketService->createTickets($reservation, $seatData);

        return $reservation;
    }

    public function updateExistingReservation(Reservation $reservation, ReservationContactInfoData $contactInfoData): void
    {
        $reservation->fillFromContactInfoData($contactInfoData);
        $this->entityManager->persist($reservation);
        $this->entityManager->flush();
    }
}
