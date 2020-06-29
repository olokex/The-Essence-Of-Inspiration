<?php declare(strict_types = 1);

namespace App\Module\Reservation\Container\ReservationSeatContainer;

use App\Module\Content\Entity\Event;
use App\Module\Hall\Repository\SeatRepository;
use App\Module\Reservation\Provider\ReservationSeatProvider\ReservationSeatProvider;
use Nette\Forms\Container;

/**
 */
class ReservationSeatContainer extends Container
{
    private const SEATS = 'seats';

    /** @var SeatRepository */
    private $seatRepository;

    public function __construct(Event $event, SeatRepository $seatRepository, ReservationSeatProvider $reservationSeatProvider)
    {
        $this->seatRepository = $seatRepository;
        $this->addCheckboxList(self::SEATS, '', $reservationSeatProvider->getAllHallSeatsForCheckboxList($event->getHall()))
            ->setDisabled($reservationSeatProvider->getReservedEventSeats($event))
            ->setRequired();
    }

    public function getValues($returnType = null): ReservationSeatData
    {
        $values = parent::getValues($returnType);

        return new ReservationSeatData(...$this->seatRepository->getByIds($values[self::SEATS]));
    }
}
