<?php declare(strict_types = 1);

namespace App\Module\Reservation\Repository;

use App\Module\Reservation\Entity\Reservation;
use App\Module\Reservation\Exception\ReservationNotFoundException;
use Doctrine\ORM\EntityManagerInterface;
use Doctrine\ORM\NoResultException;
use Doctrine\ORM\QueryBuilder;

/**
 */
class ReservationRepository
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
        $qb->select('r')->from(Reservation::class, 'r');

        return $qb;
    }

    public function getById(int $id): Reservation
    {
        try {
            $dql = <<<DQL
            SELECT r
            FROM App\Module\Reservation\Entity\Reservation r
            WHERE r.id = :reservationId
        DQL;
            $query = $this->entityManager->createQuery($dql);
            $query->setParameter('reservationId', $id);

            return $query->getSingleResult();
        } catch (NoResultException $e) {
            throw new ReservationNotFoundException("Hall with id $id has not been found.");
        }
    }
}
