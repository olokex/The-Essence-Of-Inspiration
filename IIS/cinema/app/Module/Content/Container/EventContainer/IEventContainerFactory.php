<?php declare(strict_types = 1);

namespace App\Module\Content\Container\EventContainer;

use App\Module\Content\Entity\Event;

/**
 */
interface IEventContainerFactory
{
    public function create(?Event $event = null): EventContainer;
}
