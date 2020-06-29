<?php declare(strict_types = 1);

namespace App\Module\Reservation\Component\EventSummaryControl;

use App\Module\Content\Entity\Event;

/**
 */
interface IEventSummaryControlFactory
{
    public function create(Event $event): EventSummaryControl;
}
