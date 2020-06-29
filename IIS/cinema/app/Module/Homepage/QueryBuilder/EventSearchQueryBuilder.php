<?php declare(strict_types = 1);

namespace App\Module\Homepage\QueryBuilder;

use App\Module\Content\Entity\Event;
use App\Module\Homepage\Container\EventSearchContainer\FilterData;
use Doctrine\ORM\EntityManagerInterface;
use Doctrine\ORM\QueryBuilder;

/**
 */
class EventSearchQueryBuilder
{
    /** @var EntityManagerInterface */
    private $entityManager;

    public function __construct(EntityManagerInterface $entityManager)
    {
        $this->entityManager = $entityManager;
    }

    public function build(FilterData $data): QueryBuilder
    {
        $qb = $this->getBaseQueryBuilder();
        $this->buildTitleCondition($qb, $data);
        $this->buildTypeCondition($qb, $data);
        $this->buildGenreCondition($qb, $data);
        $this->buildRatingCondition($qb, $data);

        return $qb;
    }

    private function buildTitleCondition(QueryBuilder $qb, FilterData $data): void
    {
        if ($data->getTitle()) {
            $qb->andWhere('lower(c.title) LIKE lower(:title)')
                ->setParameter('title', '%' . $data->getTitle() . '%');
        }
    }

    private function buildTypeCondition(QueryBuilder $qb, FilterData $data): void
    {
        if ($data->getType()) {
            $qb->andWhere('c.type = :typeId')
                ->setParameter('typeId', $data->getType());
        }
    }

    private function buildGenreCondition(QueryBuilder $qb, FilterData $data): void
    {
        if ($data->getGenres()) {
            $qb->join('c.genres', 'g');
            $qb->andWhere('g IN (:genreIds)')
                ->setParameter('genreIds', $data->getGenres());
        }
    }

    private function buildRatingCondition(QueryBuilder $qb, FilterData $data): void
    {
        if ($data->getRating()) {
            $qb->andWhere('c.rating = :rating')
                ->setParameter('rating', $data->getRating());
        }
    }

    private function getBaseQueryBuilder(): QueryBuilder
    {
        $qb = $this->entityManager->createQueryBuilder();
        $qb->select('e')->from(Event::class, 'e')
            ->join('e.content', 'c');

        return $qb;
    }
}
