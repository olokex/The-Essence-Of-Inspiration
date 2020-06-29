<?php declare(strict_types = 1);

namespace App\Module\Reservation\Container\ReservationContactInfoContainer;

use App\Module\User\Entity\User;

/**
 */
interface IReservationContactInfoContainerFactory
{
    public function create(?User $user = null): ReservationContactInfoContainer;
}
