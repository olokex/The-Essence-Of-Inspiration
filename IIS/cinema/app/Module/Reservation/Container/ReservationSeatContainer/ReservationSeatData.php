<?php declare(strict_types = 1);

namespace App\Module\Reservation\Container\ReservationSeatContainer;

use App\Module\Hall\Entity\Seat;

/**
 */
class ReservationSeatData
{
    /** @var Seat[] */
    private $seats;

    public function __construct(Seat ...$seats)
    {
        $this->seats = $seats;
    }

    public function getSeats(): array
    {
        return $this->seats;
    }
}
