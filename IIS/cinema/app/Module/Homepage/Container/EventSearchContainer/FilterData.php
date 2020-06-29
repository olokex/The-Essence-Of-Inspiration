<?php declare(strict_types = 1);

namespace App\Module\Homepage\Container\EventSearchContainer;

/**
 */
class FilterData
{
    /** @var string */
    private $title;

    /** @var int */
    private $type;

    /** @var array */
    private $genres;

    /** @var int */
    private $rating;

    public function __construct(?string $title, ?int $type, array $genres, ?int $rating)
    {
        $this->title = $title;
        $this->type = $type;
        $this->genres = $genres;
        $this->rating = $rating;
    }

    public function getTitle(): ?string
    {
        return $this->title;
    }

    public function getType(): ?int
    {
        return $this->type;
    }

    public function getGenres(): array
    {
        return $this->genres;
    }

    public function getRating(): ?int
    {
        return $this->rating;
    }
}
