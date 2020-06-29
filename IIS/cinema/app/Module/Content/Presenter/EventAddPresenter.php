<?php declare(strict_types = 1);

namespace App\Module\Content\Presenter;

use App\Module\Content\Component\EventFormControl\EventFormControl;
use App\Module\Content\Component\EventFormControl\IEventFormControlFactory;
use App\Module\Content\Container\EventContainer\EventData;
use App\Module\Content\Entity\Content;
use App\Module\Content\Service\EventService;
use App\Module\Front\Presenter\AbstractFrontPresenter;
use App\Module\User\Service\AccessGuard;

/**
 */
class EventAddPresenter extends AbstractFrontPresenter
{
    /** @var IEventFormControlFactory @inject */
    public $eventFormControlFactory;

    /** @var EventService @inject */
    public $eventService;

    /** @var Content */
    private $content;

    public function startup(): void
    {
        parent::startup();

        if (false === $this->user->isLoggedIn() || false === AccessGuard::hasEditorAccess($this->user->getEntity())) {
            $this->redirect(':Homepage:Homepage:default');
        }
    }

    public function actionDefault(Content $entity): void
    {
        $this->content = $entity;
    }

    protected function createComponentEventForm(): EventFormControl
    {
        $control = $this->eventFormControlFactory->create('Přidat');
        $control->onSuccess[] = function (EventData $eventData) {
            $this->eventService->addNewEvent($this->content, $eventData);
            $this->flashMessage('Událost byla vytvořena.');
            $this->redirect(':Content:ContentDetail:default', $this->content);
        };

        return $control;
    }
}
