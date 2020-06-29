<?php declare(strict_types = 1);

namespace App\Module\Content\Entity;

use App\Module\Content\Container\EventContainer\EventData;
use App\Module\Hall\Entity\Hall;
use DateTimeImmutable;
use Doctrine\ORM\Mapping as ORM;
use Package\Core\Entity\Entity;

/**
 * @ORM\Entity()
 */
class Event extends Entity
{
    public const CURRENCY_MARK = 'Kč';

    /**
     * @ORM\ManyToOne(targetEntity="Content")
     * @var Content
     */
    private $content;

    /**
     * @ORM\ManyToOne(targetEntity="App\Module\Hall\Entity\Hall")
     * @var Hall
     */
    private $hall;

    /**
     * @ORM\Column(type="float")
     * @var float
     */
    private $price;

    /**
     * @ORM\Column(type="datetime_immutable")
     * @var DateTimeImmutable
     */
    private $dateTime;

    public function __construct(Content $content, Hall $hall, float $price, \DateTimeImmutable $dateTime)
    {
        $this->content = $content;
        $this->hall = $hall;
        $this->price = $price;
        $this->dateTime = $dateTime;
    }

    public function getHall(): Hall
    {
        return $this->hall;
    }

    public function getContent(): Content
    {
        return $this->content;
    }

    public function getPrice(): float
    {
        return $this->price;
    }

    public function getFormattedPrice(): string
    {
        return $this->price . ' ' . self::CURRENCY_MARK;
    }

    public function getDateTime(): DateTimeImmutable
    {
        return $this->dateTime;
    }

    public function fillFromEventData(EventData $data): void
    {
        $this->hall = $data->getHall();
        $this->price = $data->getPrice();
        $this->dateTime = $data->getDateTime();
    }
}
