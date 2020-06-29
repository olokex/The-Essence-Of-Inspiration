<?php declare(strict_types = 1);

namespace App\Module\Hall\Service;

use App\Module\Hall\Entity\Hall;
use App\Module\Hall\Entity\Seat;
use Doctrine\ORM\EntityManagerInterface;

/**
 */
class SeatService
{
    /** @var EntityManagerInterface */
    private $entityManager;

    public function __construct(EntityManagerInterface $entityManager)
    {
        $this->entityManager = $entityManager;
    }

    public function updateSeats(Hall $hall, int $newRows, int $newSeatsInRow): void
    {
        $hallId = $hall->getId();
        [$currentRows, $currentSeatsInRow] = $this->getCurrentMaxRowAndSeatInRow($hallId);

        if ($currentRows !== $newRows || $currentSeatsInRow !== $newSeatsInRow) {
            $this->removeSeats($hall);
            $this->createSeats($hall, $newRows, $newSeatsInRow);
        }
    }

    private function createSeats(Hall $hall, int $rows, int $seatsInRow): void
    {
        foreach (\range(1, $rows) as $rowNumber) {
            foreach (\range(1, $seatsInRow) as $seatNumber) {
                $seat = new Seat($hall, $rowNumber, $seatNumber);
                $this->entityManager->persist($seat);
            }
        }
        $this->entityManager->flush();
    }

    private function removeSeats(Hall $hall): void
    {
        $dql = <<<DQL
            DELETE FROM App\Module\Hall\Entity\Seat s
            WHERE s.hall = :hall
            
        DQL;
        $query = $this->entityManager->createQuery($dql)
            ->setParameter('hall', $hall);
        $query->execute();
    }

    private function getCurrentMaxRowAndSeatInRow(int $hallId): array
    {
        $dql = <<<DQL
            SELECT MAX(s.row), MAX(s.number)
            FROM App\Module\Hall\Entity\Seat s
            WHERE s.hall = :hallId
        DQL;
        $query = $this->entityManager->createQuery($dql)->setParameter('hallId', $hallId);
        [$maxRow, $maxSeatInRow] = \array_values($query->getSingleResult());

        return [(int) $maxRow, (int) $maxSeatInRow];
    }
}
