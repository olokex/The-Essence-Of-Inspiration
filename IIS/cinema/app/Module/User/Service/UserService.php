<?php declare(strict_types = 1);

namespace App\Module\User\Service;

use App\Module\User\Container\RegistrationContainer\SettingData;
use App\Module\User\Entity\User;
use Doctrine\ORM\EntityManagerInterface;

/**
 */
class UserService
{
    /** @var EntityManagerInterface */
    private $entityManager;

    public function __construct(EntityManagerInterface $entityManager)
    {
        $this->entityManager = $entityManager;
    }

    public function saveUser(User $user, SettingData $data): void
    {
        $user->fillFromSettingData($data);

        $this->entityManager->persist($user);
        $this->entityManager->flush();
    }

    public function removeUser(User $user): void
    {
        $this->entityManager->remove($user);
        $this->entityManager->flush();
    }
}
