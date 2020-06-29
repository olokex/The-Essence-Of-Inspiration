<?php declare(strict_types = 1);

namespace App\Module\Reservation\Entity;

use App\Module\Hall\Entity\Seat;
use Doctrine\ORM\Mapping as ORM;
use Package\Core\Entity\Entity;

/**
 * @ORM\Entity()
 */
class Ticket extends Entity
{
    /**
     * @ORM\ManyToOne(targetEntity="Reservation")
     * @ORM\JoinColumn(
     *      name="reservation_id",
     *      referencedColumnName="id",
     *      onDelete="CASCADE",
     *      nullable=false
     *      )
     * @var Reservation
     */
    private $reservation;

    /**
     * @ORM\ManyToOne(targetEntity="App\Module\Hall\Entity\Seat")
     * @ORM\JoinColumn(
     *      name="seat_id",
     *      referencedColumnName="id",
     *      onDelete="SET NULL",
     *      nullable=true
     *      )
     * @var Seat
     */
    private $seat;

    public function __construct(Reservation $reservation, Seat $seat)
    {
        $this->reservation = $reservation;
        $this->seat = $seat;
    }

    public function getReservation(): Reservation
    {
        return $this->reservation;
    }

    public function getSeat(): ?Seat
    {
        return $this->seat;
    }
}
