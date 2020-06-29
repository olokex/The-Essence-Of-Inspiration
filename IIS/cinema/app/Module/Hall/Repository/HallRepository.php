<?php declare(strict_types = 1);

namespace App\Module\Hall\Repository;

use App\Module\Hall\Entity\Hall;
use App\Module\Hall\Exception\HallNotFoundException;
use Doctrine\ORM\EntityManagerInterface;
use Doctrine\ORM\NoResultException;
use Doctrine\ORM\QueryBuilder;

/**
 */
class HallRepository
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
        $qb->select('h')->from(Hall::class, 'h');

        return $qb;
    }

    public function getById(int $id): Hall
    {
        try {
            $dql = <<<DQL
            SELECT h
            FROM App\Module\Hall\Entity\Hall h
            WHERE h.id = :hallId
        DQL;
            $query = $this->entityManager->createQuery($dql);
            $query->setParameter('hallId', $id);

            return $query->getSingleResult();
        } catch (NoResultException $e) {
            throw new HallNotFoundException("Hall with id $id has not been found.");
        }
    }

    public function getPairs(): array
    {
        $dql = <<<DQL
            SELECT h.id, h.title 
            FROM App\Module\Hall\Entity\Hall h
            INDEX BY h.id
        DQL;
        $query = $this->entityManager->createQuery($dql);
        $rows = $query->getArrayResult();

        return \array_combine(\array_keys($rows), \array_column($rows, 'title'));
    }
}
