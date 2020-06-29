<?php declare(strict_types = 1);

namespace App\Module\User\Factory;

use Nette\Security\Identity;

/**
 */
interface IIdentityFactory
{
    public function create($id): Identity;
}
