<?php declare(strict_types = 1);

namespace App\Module\Content\Container\EventContainer;

use App\Module\Content\Entity\Content;
use App\Module\Hall\Entity\Hall;

/**
 */
class EventData
{
    /** @var \DateTimeImmutable */
    private $dateTime;

    /** @var float */
    private $price;

    /** @var Hall */
    private $hall;

    public function __construct(Hall $hall, \DateTimeImmutable $dateTime, float $price)
    {
        $this->hall = $hall;
        $this->dateTime = $dateTime;
        $this->price = $price;
    }

    public function getDateTime(): \DateTimeImmutable
    {
        return $this->dateTime;
    }

    public function getPrice(): float
    {
        return $this->price;
    }

    public function getHall(): Hall
    {
        return $this->hall;
    }
}
