<?php declare(strict_types = 1);

namespace App\Module\Reservation\Enum;

use Grifart\Enum\AutoInstances;
use Grifart\Enum\Enum;

/**
 * @method static ReservationStateType UNPAID()
 * @method static ReservationStateType PAID()
 */
final class ReservationStateType extends Enum
{
    use AutoInstances;

    private const
        UNPAID = 'Nezaplaceno',
        PAID = 'Zaplaceno';
}
