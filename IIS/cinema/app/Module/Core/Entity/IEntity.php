<?php declare(strict_types = 1);

namespace Package\Core\Entity;

/**
 */
interface IEntity
{
    public function getId(): int;

    public function isPersisted(): bool;
}
