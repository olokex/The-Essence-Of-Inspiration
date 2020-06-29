<?php declare(strict_types = 1);

namespace App\Module\User\Repository;

use App\Module\User\Entity\Role;
use App\Module\User\Enum\RoleMap;
use App\Module\User\Exception\RoleNotFoundException;
use Doctrine\ORM\EntityManagerInterface;
use Doctrine\ORM\NoResultException;

/**
 */
class RoleRepository
{
    /** @var EntityManagerInterface */
    private $entityManager;

    public function __construct(EntityManagerInterface $entityManager)
    {
        $this->entityManager = $entityManager;
    }

    public function findAllForSelect(): array
    {
        $dql = <<<DQL
            SELECT r.id, r.title
            FROM App\Module\User\Entity\Role r
            INDEX BY r.id
        DQL;
        $query = $this->entityManager->createQuery($dql);
        $rows = $query->getResult();

        return \array_combine(\array_keys($rows), \array_column($rows, 'title'));
    }

    public function getByMap(RoleMap $map): Role
    {
        try {
            $dql = <<<DQL
            SELECT r
            FROM App\Module\User\Entity\Role r
            WHERE r.id = :roleId
        DQL;
            $query = $this->entityManager->createQuery($dql);
            $query->setParameter('roleId', $map->toScalar());

            return $query->getSingleResult();
        } catch (NoResultException $e) {
            throw new RoleNotFoundException("User with id {$map->toScalar()} has not been found.");
        }
    }
}
