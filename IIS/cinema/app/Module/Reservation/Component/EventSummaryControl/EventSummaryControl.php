<?php declare(strict_types = 1);

namespace App\Module\Reservation\Component\EventSummaryControl;

use App\Module\Content\Entity\Event;
use App\Module\Core\Control\AbstractControl;
use App\Module\Core\Latte\TRenderable;
use Nette\Application\UI\ITemplate;

/**
 */
class EventSummaryControl extends AbstractControl
{
    use TRenderable;

    /** @var Event */
    private $event;

    public function __construct(Event $event)
    {
        $this->event = $event;
    }

    protected function setDefaultVariables(ITemplate $template): void
    {
        parent::setDefaultVariables($template);
        $template->event = $this->event;
    }
}
