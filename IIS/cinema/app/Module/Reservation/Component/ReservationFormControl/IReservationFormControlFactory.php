<?php declare(strict_types = 1);

namespace App\Module\Reservation\Component\ReservationFormControl;

use App\Module\Content\Entity\Event;
use App\Module\User\Entity\User;

/**
 */
interface IReservationFormControlFactory
{
    public function create(Event $event, ?User $userEntity = null): ReservationFormControl;
}
