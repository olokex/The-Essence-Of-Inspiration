<?php declare(strict_types = 1);

namespace App\Module\Reservation\Container\ReservationSeatContainer;

use App\Module\Content\Entity\Event;

/**
 */
interface IReservationSeatContainerFactory
{
    public function create(Event $event): ReservationSeatContainer;
}
