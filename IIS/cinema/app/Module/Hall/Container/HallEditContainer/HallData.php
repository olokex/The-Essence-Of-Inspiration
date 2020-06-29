<?php declare(strict_types = 1);

namespace App\Module\Hall\Container\HallEditContainer;

/**
 */
class HallData
{
    /** @var string */
    private $title;

    /** @var string */
    private $description;

    /** @var int */
    private $rowCount;

    /** @var int */
    private $seatInRowCount;

    public function __construct(string $title, string $description, int $rowCount, int $seatInRowCount)
    {
        $this->title = $title;
        $this->description = $description;
        $this->rowCount = $rowCount;
        $this->seatInRowCount = $seatInRowCount;
    }

    public function getTitle(): string
    {
        return $this->title;
    }

    public function getDescription(): string
    {
        return $this->description;
    }

    public function getRowCount(): int
    {
        return $this->rowCount;
    }

    public function getSeatInRowCount(): int
    {
        return $this->seatInRowCount;
    }
}
