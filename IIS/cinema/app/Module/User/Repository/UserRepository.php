<?php declare(strict_types = 1);

namespace App\Module\User\Repository;

use App\Module\User\Entity\User;
use App\Module\User\Exception\UserNotFoundException;
use Doctrine\ORM\EntityManagerInterface;
use Doctrine\ORM\NoResultException;
use Doctrine\ORM\QueryBuilder;

/**
 */
class UserRepository
{
    /** @var EntityManagerInterface */
    private $entityManager;

    public function __construct(EntityManagerInterface $entityManager)
    {
        $this->entityManager = $entityManager;
    }

    public function getQueryBuilder(): QueryBuilder
    {
        $qb = $this->entityManager->createQueryBuilder();
        $qb->select('u')->from(User::class, 'u');

        return $qb;
    }

    public function getById(int $id): User
    {
        try {
            $dql = <<<DQL
            SELECT u
            FROM App\Module\User\Entity\User u
            WHERE u.id = :userId
        DQL;
            $query = $this->entityManager->createQuery($dql);
            $query->setParameter('userId', $id);

            return $query->getSingleResult();
        } catch (NoResultException $e) {
            throw new UserNotFoundException("User with id $id has not been found.");
        }
    }

    public function findByEmail(string $email): ?User
    {
        $dql = <<<DQL
            SELECT u
            FROM App\Module\User\Entity\User u
            WHERE u.email = :email
        DQL;
        $query = $this->entityManager->createQuery($dql);
        $query->setParameter('email', $email);

        return $query->getOneOrNullResult();
    }
}
