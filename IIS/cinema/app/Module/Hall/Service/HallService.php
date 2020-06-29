<?php declare(strict_types = 1);

namespace App\Module\Hall\Service;

use App\Module\Hall\Container\HallEditContainer\HallData;
use App\Module\Hall\Entity\Hall;
use Doctrine\ORM\EntityManagerInterface;

/**
 */
class HallService
{
    /** @var EntityManagerInterface */
    private $entityManager;

    /** @var SeatService */
    private $seatService;

    public function __construct(EntityManagerInterface $entityManager, SeatService $seatService)
    {
        $this->entityManager = $entityManager;
        $this->seatService = $seatService;
    }

    public function addNewHall(HallData $hallData): Hall
    {
        $hall = new Hall($hallData->getTitle(), $hallData->getDescription());

        $this->entityManager->persist($hall);
        $this->entityManager->flush();

        $this->seatService->updateSeats($hall, $hallData->getRowCount(), $hallData->getSeatInRowCount());

        return $hall;
    }

    public function updateExistingHall(Hall $hall, HallData $hallData): void
    {
        $hall->fillFromHallData($hallData);
        $this->entityManager->persist($hall);
        $this->entityManager->flush();

        $this->seatService->updateSeats($hall, $hallData->getRowCount(), $hallData->getSeatInRowCount());
    }
}
