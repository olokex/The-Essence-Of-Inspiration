<?php declare(strict_types = 1);

namespace App\Module\User\Enum;

use Grifart\Enum\AutoInstances;
use Grifart\Enum\Enum;

/**
 * @method static RoleMap ADMIN()
 * @method static RoleMap CASHIER()
 * @method static RoleMap EDITOR()
 * @method static RoleMap CUSTOMER()
 */
final class RoleMap extends Enum
{
    use AutoInstances;

    private const
        ADMIN = 1,
        EDITOR = 2,
        CASHIER = 3,
        CUSTOMER = 4;
}
