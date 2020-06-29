<?php declare(strict_types = 1);

namespace App\Module\Content\Component\EventFormControl;

use App\Module\Content\Entity\Event;

/**
 */
interface IEventFormControlFactory
{
    public function create(string $submitCaption, ?Event $event = null): EventFormControl;
}
