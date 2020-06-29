<?php declare(strict_types = 1);

namespace App\Module\Content\Repository;

use App\Module\Content\Entity\Type;
use App\Module\Content\Exception\TypeNotFoundException;
use App\Module\User\Entity\User;
use App\Module\User\Exception\UserNotFoundException;
use Doctrine\ORM\EntityManagerInterface;
use Doctrine\ORM\NoResultException;

/**
 */
class TypeRepository
{
    /** @var EntityManagerInterface */
    private $entityManager;

    public function __construct(EntityManagerInterface $entityManager)
    {
        $this->entityManager = $entityManager;
    }

    public function getById(int $id): Type
    {
        try {
            $dql = <<<DQL
            SELECT t
            FROM App\Module\Content\Entity\Type t
            WHERE t.id = :typeId
        DQL;
            $query = $this->entityManager->createQuery($dql);
            $query->setParameter('typeId', $id);

            return $query->getSingleResult();
        } catch (NoResultException $e) {
            throw new TypeNotFoundException("Type with id $id has not been found.");
        }
    }

    public function getPairs(): array
    {
        $dql = <<<DQL
            SELECT t.id, t.title 
            FROM App\Module\Content\Entity\Type t
            INDEX BY t.id
        DQL;
        $query = $this->entityManager->createQuery($dql);
        $rows = $query->getArrayResult();

        return \array_combine(\array_keys($rows), \array_column($rows, 'title'));
    }
}
