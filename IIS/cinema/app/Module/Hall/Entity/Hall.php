<?php declare(strict_types = 1);

namespace App\Module\Hall\Entity;

use App\Module\Hall\Container\HallEditContainer\HallData;
use Doctrine\Common\Collections\ArrayCollection;
use Doctrine\ORM\Mapping as ORM;
use Package\Core\Entity\Entity;
use Doctrine\Common\Collections\Collection;

/**
 * @ORM\Entity()
 */
class Hall extends Entity
{
    /**
     * @ORM\Column(type="string")
     * @var string
     */
    private $title;

    /**
     * @ORM\Column(type="string")
     * @var string
     */
    private $description;

    /**
     * @ORM\OneToMany(targetEntity="Seat", mappedBy="hall")
     * @var Collection
     */
    private $seats;

    public function __construct(string $title, string $description)
    {
        $this->title = $title;
        $this->description = $description;
        $this->seats = new ArrayCollection();
    }

    public function getTitle(): string
    {
        return $this->title;
    }

    public function getDescription(): string
    {
        return $this->description;
    }

    public function getSeats(): Collection
    {
        return $this->seats;
    }

    public function getRowCount(): int
    {
        $count = 0;

        /** @var Seat $seat */
        foreach ($this->getSeats() as $seat) {
            $row = $seat->getRow();

            if ($row > $count) {
                $count = $row;
            }
        }

        return $count;
    }

    public function getSeatInRowCount(): int
    {
        $count = 0;

        /** @var Seat $seat */
        foreach ($this->getSeats() as $seat) {
            $number = $seat->getNumber();

            if ($number > $count) {
                $count = $number;
            }
        }

        return $count;
    }

    public function fillFromHallData(HallData $hallData): void
    {
        $this->title = $hallData->getTitle();
        $this->description = $hallData->getDescription();
    }
}
