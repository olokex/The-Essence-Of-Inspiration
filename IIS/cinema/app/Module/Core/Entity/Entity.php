<?php declare(strict_types = 1);

namespace Package\Core\Entity;

use Doctrine\ORM\Mapping as ORM;

/**
 */
abstract class Entity implements IEntity
{
    /**
     * @var int
     * @ORM\Column(type="integer", nullable=FALSE)
     * @ORM\Id
     * @ORM\GeneratedValue
     */
    protected $id;

    public function getId(): int
    {
        return $this->id;
    }

    public function isPersisted(): bool
    {
        return null !== $this->id;
    }

    public function __clone()
    {
        $this->id = null;
    }
}
