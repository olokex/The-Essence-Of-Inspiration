<?php declare(strict_types = 1);

namespace App\Module\Reservation\Component\ContactInfoSummaryControl;

use App\Module\Reservation\Entity\Reservation;

/**
 */
interface IContactInfoSummaryControlFactory
{
    public function create(Reservation $reservation): ContactInfoSummaryControl;
}
