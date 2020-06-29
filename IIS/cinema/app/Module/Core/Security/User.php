<?php declare(strict_types = 1);

namespace App\Module\Core\Security;

use App\Module\User\Entity\User as UserEntity;
use App\Module\User\Repository\UserRepository;
use Nette\Security\IAuthenticator;
use Nette\Security\IAuthorizator;
use Nette\Security\IUserStorage;
use Nette\Security\User as NwUser;

/**
 */
class User extends NwUser
{
    /** @var UserRepository */
    private $userRepository;

    public function __construct(
        IUserStorage $storage,
        UserRepository $userRepository,
        IAuthenticator $authenticator = null,
        IAuthorizator $authorizator = null
    )
    {
        parent::__construct($storage, $authenticator, $authorizator);
        $this->userRepository = $userRepository;
    }

    public function getEntity(): ?UserEntity
    {
        $id = $this->getId();

        return $id ? $this->userRepository->getById($id) : null;
    }
}
