<?php declare(strict_types = 1);

namespace App\Module\Reservation\Component\TicketSummaryControl;

use App\Module\Core\Control\AbstractControl;
use App\Module\Core\Latte\TRenderable;
use App\Module\Reservation\Entity\Reservation;
use Nette\Application\UI\ITemplate;

/**
 */
class TicketSummaryControl extends AbstractControl
{
    use TRenderable;

    /** @var Reservation */
    private $reservation;

    public function __construct(Reservation $reservation)
    {
        $this->reservation = $reservation;
    }

    protected function setDefaultVariables(ITemplate $template): void
    {
        parent::setDefaultVariables($template);
        $template->tickets = $this->reservation->getTickets();
    }
}
