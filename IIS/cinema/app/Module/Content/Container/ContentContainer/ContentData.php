<?php declare(strict_types = 1);

namespace App\Module\Content\Container\ContentContainer;

use App\Module\Content\Entity\Type;
use Doctrine\Common\Collections\Collection;
use WebChemistry\Images\Resources\Transfer\UploadResource;

/**
 */
class ContentData
{
    /** @var string */
    private $title;

    /** @var string */
    private $description;

    /** @var Type */
    private $type;

    /** @var Collection */
    private $genres;

    /** @var int */
    private $rating;

    /** @var int */
    private $length;

    /** @var string */
    private $performers;

    /** @var UploadResource|null */
    private $uploadResource;

    public function __construct(string $title, string $description, Type $type, Collection $genres, int $rating, int $length, string $performers, ?UploadResource $uploadResource)
    {
        $this->title = $title;
        $this->description = $description;
        $this->type = $type;
        $this->genres = $genres;
        $this->rating = $rating;
        $this->length = $length;
        $this->performers = $performers;
        $this->uploadResource = $uploadResource;
    }

    public function getTitle(): string
    {
        return $this->title;
    }

    public function getDescription(): string
    {
        return $this->description;
    }

    public function getType(): Type
    {
        return $this->type;
    }

    public function getGenres(): Collection
    {
        return $this->genres;
    }

    public function getRating(): int
    {
        return $this->rating;
    }

    public function getLength(): int
    {
        return $this->length;
    }

    public function getPerformers(): string
    {
        return $this->performers;
    }

    public function getUploadResource(): ?UploadResource
    {
        return $this->uploadResource;
    }
}
