<?php declare(strict_types = 1);

namespace App\Module\Hall\Repository;

use App\Module\Hall\Exception\SeatNotFoundException;
use Doctrine\ORM\EntityManagerInterface;
use Doctrine\ORM\NoResultException;

/**
 */
class SeatRepository
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
            SELECT s
            FROM App\Module\Hall\Entity\Seat s
            WHERE s.id IN (:seatIds) 
        DQL;
            $query = $this->entityManager->createQuery($dql);
            $query->setParameter('seatIds', $ids);

            return $query->getResult();
        } catch (NoResultException $e) {
            throw new SeatNotFoundException('No types have been found.');
        }
    }
}
