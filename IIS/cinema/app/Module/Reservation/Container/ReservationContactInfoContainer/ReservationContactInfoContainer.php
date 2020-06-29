<?php declare(strict_types = 1);

namespace App\Module\Reservation\Container\ReservationContactInfoContainer;

use App\Module\User\Entity\User;
use Nette\Application\UI\Form;
use Nette\Forms\Container;

/**
 */
class ReservationContactInfoContainer extends Container
{
    private const
        FIRST_NAME = 'firstName',
        LAST_NAME = 'lastName',
        EMAIL = 'email',
        PHONE = 'phone';

    /** @var User|null */
    private $user;

    public function __construct(?User $user = null)
    {
        $this->user = $user;

        $this->addText(self::FIRST_NAME, 'Jméno')
            ->setHtmlAttribute('placeholder', 'Jméno')
            ->setRequired();
        $this->addText(self::LAST_NAME, 'Příjmení')
            ->setHtmlAttribute('placeholder', 'Příjmení')
            ->setRequired();
        $this->addEmail(self::EMAIL, 'E-mail')
            ->setHtmlAttribute('placeholder', 'E-mail')
            ->setRequired();
        $this->addText(self::PHONE, 'Telefon')
            ->addRule(Form::MIN, 'Délka tel. čísla musí mít alespoň 9 znaků', 9)
            ->setHtmlAttribute('placeholder', 'Telefon')
            ->setRequired();

        if ($user) {
            $this->setDefaults([
                self::FIRST_NAME => $user->getFirstName(),
                self::LAST_NAME => $user->getLastName(),
                self::EMAIL => $user->getEmail(),
                self::PHONE => $user->getPhone(),
            ]);
        }
    }

    public function getValues($returnType = null): ReservationContactInfoData
    {
        $values = parent::getValues($returnType);

        return new ReservationContactInfoData(
            $values[self::FIRST_NAME],
            $values[self::LAST_NAME],
            $values[self::EMAIL],
            (string) $values[self::PHONE],
            $this->user
        );
    }
}
