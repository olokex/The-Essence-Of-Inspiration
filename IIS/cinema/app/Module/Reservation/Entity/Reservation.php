<?php declare(strict_types = 1);

namespace App\Module\Reservation\Entity;

use App\Module\Content\Entity\Event;
use App\Module\Reservation\Container\ReservationContactInfoContainer\ReservationContactInfoData;
use App\Module\Reservation\Enum\ReservationStateType;
use App\Module\User\Entity\User;
use Doctrine\Common\Collections\ArrayCollection;
use Doctrine\Common\Collections\Collection;
use Package\Core\Entity\Entity;
use Doctrine\ORM\Mapping as ORM;

/**
 * @ORM\Entity()
 */
class Reservation extends Entity
{
    /**
     * @ORM\ManyToOne(targetEntity="App\Module\User\Entity\User")
     * @ORM\JoinColumn(
     *      name="user_id",
     *      referencedColumnName="id",
     *      onDelete="CASCADE",
     *      nullable=false
     *      )
     * @var User|null
     */
    private $user;

    /**
     * @ORM\Column(type="string")
     * @var string
     */
    private $firstName;

    /**
     * @ORM\Column(type="string")
     * @var string
     */
    private $lastName;

    /**
     * @ORM\Column(type="string")
     * @var string
     */
    private $email;

    /**
     * @ORM\Column(type="string")
     * @var string
     */
    private $phone;

    /**
     * @ORM\Column(type="string", nullable=true)
     * @var string
     */
    private $state;

    /**
     * @ORM\OneToMany(targetEntity="Ticket", mappedBy="reservation")
     * @var Collection
     */
    private $tickets;

    /**
     * @ORM\ManyToOne(targetEntity="App\Module\Content\Entity\Event")
     * @var Event
     */
    private $event;

    public function __construct(string $firstName, string $lastName, string $email, string $phone, Event $event, ReservationStateType $reservationStateType, ?User $user = null)
    {
        $this->firstName = $firstName;
        $this->lastName = $lastName;
        $this->email = $email;
        $this->phone = $phone;
        $this->event = $event;
        $this->user = $user;
        $this->state = $reservationStateType->toScalar();
        $this->tickets = new ArrayCollection();
    }

    public function getUser(): ?User
    {
        return $this->user;
    }

    public function getFirstName(): string
    {
        return $this->firstName;
    }

    public function getLastName(): string
    {
        return $this->lastName;
    }

    public function getEmail(): string
    {
        return $this->email;
    }

    public function getPhone(): string
    {
        return $this->phone;
    }

    public function getTickets(): Collection
    {
        return $this->tickets;
    }

    public function setTickets(Collection $tickets): void
    {
        $this->tickets = $tickets;
    }

    public function getEvent(): Event
    {
        return $this->event;
    }

    public function getSeatIds(): array
    {
        $ret = [];

        /** @var Ticket $ticket */
        foreach ($this->getTickets() as $ticket) {
            $ret[] = $ticket->getSeat()->getId();
        }

        return $ret;
    }

    public function getPrice(): float
    {
        return $this->event->getPrice() * $this->getTickets()->count();
    }

    public function getState(): ReservationStateType
    {
        return ReservationStateType::fromScalar($this->state);
    }

    public function getStateAsString(): string
    {
        return $this->state;
    }

    public function setState(ReservationStateType $type): void
    {
        $this->state = $type->toScalar();
    }

    public function getFormattedPrice(): string
    {
        $price = $this->getPrice();

        return $price . ' ' . Event::CURRENCY_MARK;
    }

    public function fillFromContactInfoData(ReservationContactInfoData $data): void
    {
        $this->firstName = $data->getFirstName();
        $this->lastName = $data->getLastName();
        $this->email = $data->getEmail();
        $this->phone = $data->getPhone();
    }
}
