<?php declare(strict_types = 1);

namespace App\Module\Content\Entity;

use App\Module\Content\Container\ContentContainer\ContentData;
use Doctrine\Common\Collections\Collection;
use Doctrine\ORM\Mapping as ORM;
use Package\Core\Entity\Entity;

/**
 * @ORM\Entity()
 */
class Content extends Entity
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
     * @ORM\ManyToOne(targetEntity="Type")
     * @var Type
     */
    private $type;

    /**
     * @ORM\ManyToMany(targetEntity="Genre", indexBy="id")
     * @ORM\JoinTable(name="event_genre",
     *      joinColumns={@ORM\JoinColumn(name="event_id", referencedColumnName="id")},
     *      inverseJoinColumns={@ORM\JoinColumn(name="genre_id", referencedColumnName="id")}
     *      )
     * @var Collection
     */
    private $genres;

    /**
     * @ORM\Column(type="string", nullable=true)
     * @var string
     */
    private $image;

    /**
     * @ORM\Column(type="string")
     * @var string
     */
    private $performers;

    /**
     * @ORM\Column(type="integer")
     * @var int
     */
    private $rating;

    /**
     * @ORM\Column(type="integer")
     * @var int
     */
    private $length;

    public function __construct(string $title, string $description, Type $type, Collection $genres, int $rating, int $length, string $performers)
    {
        $this->title = $title;
        $this->description = $description;
        $this->type = $type;
        $this->genres = $genres;
        $this->rating = $rating;
        $this->length = $length;
        $this->performers = $performers;
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

    public function getImage(): ?string
    {
        return $this->image;
    }

    public function getPerformers(): string
    {
        return $this->performers;
    }

    public function getRating(): int
    {
        return $this->rating;
    }

    public function getLength(): int
    {
        return $this->length;
    }

    public function getGenresAsString(): string
    {
        return \implode(', ', \array_map(static function(Genre $genre) {
            return $genre->getTitle();
        }, $this->getGenres()->toArray()));
    }

    public function getGenreIds(): array
    {
        $ret = [];

        /** @var Genre $genre */
        foreach ($this->getGenres() as $genre) {
            $ret[] = $genre->getId();
        }

        return $ret;
    }

    public function fillFromContentData(ContentData $data): void
    {
        $this->title = $data->getTitle();
        $this->description = $data->getDescription();
        $this->type = $data->getType();
        $this->genres = $data->getGenres();
        $this->rating = $data->getRating();
        $this->length = $data->getLength();
        $this->performers = $data->getPerformers();
    }

    public function setImage(string $image): void
    {
        $this->image = $image;
    }
}
