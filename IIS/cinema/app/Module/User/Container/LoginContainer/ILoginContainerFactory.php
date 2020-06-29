<?php declare(strict_types = 1);

namespace App\Module\User\Container\LoginContainer;

/**
 */
interface ILoginContainerFactory
{
    public function create(): LoginContainer;
}
