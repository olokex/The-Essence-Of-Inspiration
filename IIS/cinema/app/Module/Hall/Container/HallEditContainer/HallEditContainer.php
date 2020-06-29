<?php declare(strict_types = 1);

namespace App\Module\Hall\Container\HallEditContainer;

use App\Module\Hall\Entity\Hall;
use Nette\Application\UI\Form;
use Nette\Forms\Container;

/**
 */
class HallEditContainer extends Container
{
    private const
        TITLE = 'title',
        DESCRIPTION = 'description',
        ROW_COUNT = 'rowCount',
        SEAT_IN_ROW_COUNT = 'seatInRowCount';

    public function __construct(?Hall $hall = null)
    {
        $this->addText(self::TITLE, 'Název')
            ->setHtmlAttribute('placeholder', 'Název');
        $this->addTextArea(self::DESCRIPTION, 'Popis')
            ->setHtmlAttribute('placeholder', 'Popis');
        $this->addInteger(self::ROW_COUNT, 'Počet řad')
            ->setHtmlAttribute('placeholder', 'Počet řad')
            ->addRule(Form::MIN, 'Nejmenší povolená hodnota je 1.', 1)
            ->setRequired();
        $this->addInteger(self::SEAT_IN_ROW_COUNT, 'Počet sedadel v řadě')
            ->setHtmlAttribute('placeholder', 'Počet sedadel v řadě')
            ->addRule(Form::MIN, 'Nejmenší povolená hodnota je 1.', 1)
            ->setRequired();

        if ($hall) {
            $this->setDefaults([
                self::TITLE => $hall->getTitle(),
                self::DESCRIPTION => $hall->getDescription(),
                self::ROW_COUNT => $hall->getRowCount(),
                self::SEAT_IN_ROW_COUNT => $hall->getSeatInRowCount(),
            ]);
        }
    }

    public function getValues($returnType = null): HallData
    {
        $values = parent::getValues($returnType);

        return new HallData($values[self::TITLE], $values[self::DESCRIPTION], $values[self::ROW_COUNT], $values[self::SEAT_IN_ROW_COUNT]);
    }
}
