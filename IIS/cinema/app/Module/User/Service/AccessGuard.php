<?php declare(strict_types = 1);

namespace App\Module\User\Service;

use App\Module\User\Entity\User;
use App\Module\User\Enum\RoleMap;
use Nette\StaticClass;

/**
 */
class AccessGuard
{
    use StaticClass;

    public static function hasAdminAccess(User $user): bool
    {
        return $user->getRole()->getMap() === RoleMap::ADMIN();
    }

    public static function hasEditorAccess(User $user): bool
    {
        $role = $user->getRole()->getMap();

        return RoleMap::EDITOR() === $role || RoleMap::ADMIN() === $role;
    }

    public static function hasCashierAccess(User $user): bool
    {
        $role = $user->getRole()->getMap();

        return RoleMap::CASHIER() === $role || RoleMap::ADMIN() === $role;
    }
}
