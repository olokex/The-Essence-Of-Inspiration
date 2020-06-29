<?php declare(strict_types = 1);

namespace App\Module\Hall\Entity;

use Doctrine\ORM\Mapping as ORM;
use Package\Core\Entity\Entity;

/**
 * @ORM\Entity()
 */
class Seat extends Entity
{
    /**
     * @ORM\ManyToOne(targetEntity="Hall")
     * @var Hall
     */
    private $hall;

    /**
     * @ORM\Column(type="integer")
     * @var int
     */
    private $row;

    /**
     * @ORM\Column(type="integer")
     * @var int
     */
    private $number;

    public function __construct(Hall $hall, int $row, int $number)
    {
        $this->hall = $hall;
        $this->row = $row;
        $this->number = $number;
    }

    public function getHall(): Hall
    {
        return $this->hall;
    }

    public function getRow(): int
    {
        return $this->row;
    }

    public function getNumber(): int
    {
        return $this->number;
    }
}
