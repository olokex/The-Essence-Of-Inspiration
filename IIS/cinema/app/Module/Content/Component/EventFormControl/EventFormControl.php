<?php declare(strict_types = 1);

namespace App\Module\Content\Component\EventFormControl;

use App\Module\Content\Container\EventContainer\EventData;
use App\Module\Content\Container\EventContainer\IEventContainerFactory;
use App\Module\Content\Entity\Event;
use App\Module\Core\Control\AbstractControl;
use App\Module\Core\Form\FormFactory;
use App\Module\Core\Latte\TRenderable;
use Nette\Application\UI\Form;

/**
 * @method onSuccess(EventData $eventData)
 */
class EventFormControl extends AbstractControl
{
    use TRenderable;

    private const COMPONENT_EVENT = 'event';

    /** @var callable[] */
    public $onSuccess = [];

    /** @var FormFactory */
    private $formFactory;

    /** @var Event|null */
    private $event;

    /** @var IEventContainerFactory */
    private $eventContainerFactory;

    /** @var string */
    private $submitCaption;

    public function __construct(string $submitCaption, IEventContainerFactory $eventContainerFactory, FormFactory $formFactory, ?Event $event = null)
    {
        $this->formFactory = $formFactory;
        $this->eventContainerFactory = $eventContainerFactory;
        $this->event = $event;
        $this->submitCaption = $submitCaption;
    }

    protected function createComponentForm(): Form
    {
        $form = $this->formFactory->create();
        $form->addComponent($this->eventContainerFactory->create($this->event), self::COMPONENT_EVENT);
        $form->addSubmit('send', $this->submitCaption);
        $form->onSuccess[] = [$this, 'process'];

        return $form;
    }

    public function process(Form $form): void
    {
        /** @var EventData $eventData */
        $eventData = $form[self::COMPONENT_EVENT]->getValues();
        $this->onSuccess($eventData);
    }
}
