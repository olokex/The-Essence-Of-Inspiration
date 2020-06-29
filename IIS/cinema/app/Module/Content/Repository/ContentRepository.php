<?php declare(strict_types = 1);

namespace App\Module\Content\Repository;

use App\Module\Content\Entity\Content;
use App\Module\Content\Exception\ContentNotFoundException;
use Doctrine\ORM\EntityManagerInterface;
use Doctrine\ORM\NoResultException;
use Doctrine\ORM\QueryBuilder;

/**
 */
class ContentRepository
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
        $qb->select('c')->from(Content::class, 'c');

        return $qb;
    }

    public function getById(int $id): Content
    {
        try {
            $dql = <<<DQL
            SELECT c
            FROM App\Module\Content\Entity\Content c
            WHERE c.id = :contentId
        DQL;
            $query = $this->entityManager->createQuery($dql);
            $query->setParameter('contentId', $id);

            return $query->getSingleResult();
        } catch (NoResultException $e) {
            throw new ContentNotFoundException("Content with id $id has not been found.");
        }
    }
}
