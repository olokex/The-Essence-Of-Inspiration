<?php declare(strict_types = 1);

namespace App\Module\User\Container\RegistrationContainer;

/**
 */
interface IRegistrationContainerFactory
{
    public function create(): RegistrationContainer;
}
