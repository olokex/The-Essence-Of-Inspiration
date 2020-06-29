<?php declare(strict_types = 1);

namespace App\Module\Content\Repository;

use App\Module\Content\Exception\TypeNotFoundException;
use Doctrine\ORM\EntityManagerInterface;
use Doctrine\ORM\NoResultException;

/**
 */
class GenreRepository
{
    /** @var EntityManagerInterface */
    private $entityManager;

    public function __construct(EntityManagerInterface $entityManager)
    {
        $this->entityManager = $entityManager;
    }

    public function getByIds(array $ids): array
    {
        try {
            $dql = <<<DQL
            SELECT t
            FROM App\Module\Content\Entity\Genre t
            WHERE t.id IN (:genreIds) 
        DQL;
            $query = $this->entityManager->createQuery($dql);
            $query->setParameter('genreIds', $ids);

            return $query->getResult();
        } catch (NoResultException $e) {
            throw new TypeNotFoundException('No types have been found.');
        }
    }

    public function getPairs(): array
    {
        $dql = <<<DQL
            SELECT g.id, g.title 
            FROM App\Module\Content\Entity\Genre g
            INDEX BY g.id
        DQL;
        $query = $this->entityManager->createQuery($dql);
        $rows = $query->getArrayResult();

        return \array_combine(\array_keys($rows), \array_column($rows, 'title'));
    }
}
