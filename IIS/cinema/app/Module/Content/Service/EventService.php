<?php declare(strict_types = 1);

namespace App\Module\Content\Service;

use App\Module\Content\Container\EventContainer\EventData;
use App\Module\Content\Entity\Content;
use App\Module\Content\Entity\Event;
use Doctrine\ORM\EntityManagerInterface;

/**
 */
class EventService
{
    /** @var EntityManagerInterface */
    private $entityManager;

    public function __construct(EntityManagerInterface $entityManager)
    {
        $this->entityManager = $entityManager;
    }

    public function addNewEvent(Content $content, EventData $data): Event
    {
        $event = new Event(
            $content,
            $data->getHall(),
            $data->getPrice(),
            $data->getDateTime()
        );
        $this->entityManager->persist($event);
        $this->entityManager->flush();

        return $event;
    }

    public function updateExistingEvent(Event $event, EventData $eventData): void
    {
        $event->fillFromEventData($eventData);
        $this->entityManager->persist($event);
        $this->entityManager->flush();
    }
}
