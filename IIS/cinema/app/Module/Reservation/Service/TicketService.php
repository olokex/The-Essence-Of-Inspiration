<?php declare(strict_types = 1);

namespace App\Module\Reservation\Service;

use App\Module\Reservation\Container\ReservationSeatContainer\ReservationSeatData;
use App\Module\Reservation\Entity\Reservation;
use App\Module\Reservation\Entity\Ticket;
use Doctrine\ORM\EntityManagerInterface;

/**
 */
class TicketService
{
    /** @var EntityManagerInterface */
    private $entityManager;

    public function __construct(EntityManagerInterface $entityManager)
    {
        $this->entityManager = $entityManager;
    }

    public function createTickets(Reservation $reservation, ReservationSeatData $seatData): void
    {
        foreach ($seatData->getSeats() as $seat) {
            $ticket = new Ticket($reservation, $seat);
            $this->entityManager->persist($ticket);
        }
        $this->entityManager->flush();
    }
}
