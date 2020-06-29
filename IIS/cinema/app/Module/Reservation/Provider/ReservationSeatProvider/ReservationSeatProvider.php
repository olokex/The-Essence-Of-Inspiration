<?php declare(strict_types = 1);

namespace App\Module\Reservation\Provider\ReservationSeatProvider;

use App\Module\Content\Entity\Event;
use App\Module\Hall\Entity\Hall;
use Doctrine\ORM\EntityManagerInterface;

/**
 */
class ReservationSeatProvider
{
    /** @var EntityManagerInterface */
    private $entityManager;

    public function __construct(EntityManagerInterface $entityManager)
    {
        $this->entityManager = $entityManager;
    }

    public function getReservedEventSeats(Event $event): array
    {
        $dql = <<<DQL
            SELECT IDENTITY(t.seat)
            FROM App\Module\Reservation\Entity\Ticket t
                JOIN t.reservation r
                JOIN r.event e
            WHERE e = :event
        DQL;
        $query = $this->entityManager->createQuery($dql)
            ->setParameter('event', $event);
        $seats = $query->getArrayResult();
        $ret = [];

        foreach ($seats as $seat) {
            $ret[] = \reset($seat);
        }

        return $ret;
    }

    public function getMaxSeatInRow(Hall $hall): int
    {
        $dql = <<<DQL
            SELECT MAX(s.number)
            FROM App\Module\Hall\Entity\Seat s
            WHERE s.hall = :hall
        DQL;
        $query = $this->entityManager->createQuery($dql)->setParameter('hall', $hall);

        return (int) $query->getSingleScalarResult();
    }

    public function getAllHallSeatsForCheckboxList(Hall $hall): array
    {
        $dql = <<<DQL
            SELECT s.id
            FROM App\Module\Hall\Entity\Seat s
            INDEX BY s.id
            WHERE s.hall = :hall
            ORDER BY s.row ASC, s.number ASC
        DQL;
        $query = $this->entityManager->createQuery($dql)
            ->setParameter('hall', $hall);
        $seats = \array_keys($query->getResult());
        $ret = [];

        foreach ($seats as $seat) {
            $ret[$seat] = '';
        }

        return $ret;
    }

    public function getAllHallSeatsForTemplate(Hall $hall): array
    {
        $dql = <<<DQL
            SELECT s.id, s.row
            FROM App\Module\Hall\Entity\Seat s
            INDEX BY s.id
            WHERE s.hall = :hall
            ORDER BY s.row ASC, s.number ASC
        DQL;
        $query = $this->entityManager->createQuery($dql)
            ->setParameter('hall', $hall);
        $seats = $query->getResult();
        $ret = [];

        foreach ($seats as $key => $seat) {
            $ret[$seat['row']][$key] = '';
        }

        return $ret;
    }
}
