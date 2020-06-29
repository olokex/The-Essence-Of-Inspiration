<?php declare(strict_types = 1);

namespace App\Module\Content\Repository;

use App\Module\Content\Entity\Event;
use App\Module\Content\Exception\EventNotFoundException;
use Doctrine\ORM\EntityManagerInterface;
use Doctrine\ORM\NoResultException;
use Doctrine\ORM\QueryBuilder;

/**
 */
class EventRepository
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
        $qb->select('e')->from(Event::class, 'e');

        return $qb;
    }

    public function getById(int $id): Event
    {
        try {
            $dql = <<<DQL
            SELECT e
            FROM App\Module\Content\Entity\Event e
            WHERE e.id = :eventId
        DQL;
            $query = $this->entityManager->createQuery($dql);
            $query->setParameter('eventId', $id);

            return $query->getSingleResult();
        } catch (NoResultException $e) {
            throw new EventNotFoundException("Event with id $id has not been found.");
        }
    }
}
