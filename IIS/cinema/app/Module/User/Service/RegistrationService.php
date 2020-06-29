<?php declare(strict_types = 1);

namespace App\Module\User\Service;

use App\Module\User\Container\RegistrationContainer\RegistrationData;
use App\Module\User\Entity\User;
use App\Module\User\Enum\RoleMap;
use App\Module\User\Repository\RoleRepository;
use Doctrine\ORM\EntityManagerInterface;
use Nette\Security\Passwords;

/**
 */
class RegistrationService
{
    /** @var EntityManagerInterface */
    private $entityManager;

    /** @var Passwords */
    private $passwords;

    /** @var RoleRepository */
    private $roleRepository;

    public function __construct(EntityManagerInterface $entityManager, Passwords $passwords, RoleRepository $roleRepository)
    {
        $this->entityManager = $entityManager;
        $this->passwords = $passwords;
        $this->roleRepository = $roleRepository;
    }

    public function register(RegistrationData $data): void
    {
        $user = $this->createUser($data);
        $this->entityManager->persist($user);
        $this->entityManager->flush();
    }

    private function createUser(RegistrationData $data): User
    {
        return new User(
            $data->getFirstName(),
            $data->getLastName(),
            $data->getEmail(),
            $data->getPhone(),
            $this->passwords->hash($data->getPassword()),
            $this->roleRepository->getByMap(RoleMap::CUSTOMER()),
        );
    }
}
