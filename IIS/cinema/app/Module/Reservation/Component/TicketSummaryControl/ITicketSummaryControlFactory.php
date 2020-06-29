<?php declare(strict_types = 1);

namespace App\Module\Reservation\Component\TicketSummaryControl;

use App\Module\Reservation\Entity\Reservation;

/**
 */
interface ITicketSummaryControlFactory
{
    public function create(Reservation $reservation): TicketSummaryControl;
}
