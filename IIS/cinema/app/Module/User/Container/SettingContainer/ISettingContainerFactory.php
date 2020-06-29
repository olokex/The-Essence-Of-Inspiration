<?php declare(strict_types = 1);

namespace App\Module\User\Container\SettingContainer;

use App\Module\User\Entity\User;

/**
 */
interface ISettingContainerFactory
{
    public function create(User $userEntity): SettingContainer;
}
