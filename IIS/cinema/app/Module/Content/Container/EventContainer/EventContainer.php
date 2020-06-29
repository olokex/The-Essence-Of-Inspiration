<?php declare(strict_types = 1);

namespace App\Module\Content\Container\EventContainer;

use App\Module\Content\Entity\Event;
use App\Module\Hall\Repository\HallRepository;
use Nette\Application\UI\Form;
use Nette\Forms\Container;

/**
 */
class EventContainer extends Container
{
    private const
        DATE_TIME = 'dateTime',
        HALL = 'hall',
        PRICE = 'price';

    /** @var HallRepository */
    private $hallRepository;

    public function __construct(HallRepository $hallRepository, ?Event $event = null)
    {
        $this->hallRepository = $hallRepository;

        $this->addDateTimePicker(self::DATE_TIME, 'Datum a čas')
            ->setRequired();
        $this->addSelect(self::HALL, 'Sál', $hallRepository->getPairs());
        $this->addInteger(self::PRICE, 'Cena')
            ->addRule(Form::FLOAT)
            ->setRequired();

        if ($event) {
            $this->setDefaults([
                self::DATE_TIME => $event->getDateTime(),
                self::HALL => $event->getHall()->getId(),
                self::PRICE => $event->getPrice(),
            ]);
        }
    }

    public function getValues($returnType = null): EventData
    {
        $values = parent::getValues('array');

        return new EventData(
            $this->hallRepository->getById($values[self::HALL]),
            $values[self::DATE_TIME],
            $values[self::PRICE]
        );
    }
}
