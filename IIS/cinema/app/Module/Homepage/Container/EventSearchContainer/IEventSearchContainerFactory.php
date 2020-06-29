<?php declare(strict_types = 1);

namespace App\Module\Homepage\Container\EventSearchContainer;

/**
 */
interface IEventSearchContainerFactory
{
    public function create(array $defaults): EventSearchContainer;
}
