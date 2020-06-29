<?php declare(strict_types = 1);

namespace App\Module\User\Service;

use App\Module\User\Component\LoginContainer\LoginData;
use App\Module\User\Factory\IIdentityFactory;
use App\Module\User\Repository\UserRepository;
use Nette\Security\Passwords;
use Nette\Security\User;

/**
 */
class LoginAuthenticator
{
    /** @var User */
    private $user;

    /** @var UserRepository */
    private $userRepository;

    /** @var Passwords */
    private $passwords;

    /** @var IIdentityFactory */
    private $identityFactory;

    public function __construct(
        User $user,
        UserRepository $userRepository,
        Passwords $passwords,
        IIdentityFactory $identityFactory
    )
    {
        $this->user = $user;
        $this->userRepository = $userRepository;
        $this->passwords = $passwords;
        $this->identityFactory = $identityFactory;
    }

    public function authenticate(LoginData $loginData): bool
    {
        $ret = false;
        $user = $this->userRepository->findByEmail($loginData->getEmail());

        if ($user && $this->passwords->verify($loginData->getPassword(), $user->getPassword())) {
            $identity = $this->identityFactory->create($user->getId());
            $this->user->login($identity);
            $ret = true;
        }

        return $ret;
    }
}
