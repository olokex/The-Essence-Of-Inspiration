<?php declare(strict_types = 1);

namespace App\Module\User\Container\RegistrationContainer;

use Nette\Application\UI\Form;
use Nette\Forms\Container;

/**
 */
class RegistrationContainer extends Container
{
    public const
        FIRST_NAME = 'firstName',
        LAST_NAME = 'lastName',
        EMAIL = 'email',
        PHONE = 'phone',
        PASSWORD = 'password',
        PASSWORD_CONFIRMATION = 'passwordConfirmation';

    public function __construct()
    {
        $this->addText(self::FIRST_NAME)
            ->setHtmlAttribute('placeholder', 'Jméno')
            ->setRequired();

        $this->addText(self::LAST_NAME)
            ->setHtmlAttribute('placeholder', 'Příjmení')
            ->setRequired();

        $this->addEmail(self::EMAIL)
            ->setHtmlAttribute('placeholder', 'E-mail')
            ->setRequired();

        $this->addInteger(self::PHONE)
            ->setHtmlAttribute('placeholder', 'Telefon')
            ->addRule(Form::MIN, 'Délka tel. čísla musí mít alespoň 9 znaků', 9)
            ->setRequired();

        $password = $this->addPassword(self::PASSWORD)
            ->setHtmlAttribute('placeholder', 'Heslo')
            ->setRequired();
        $password->addRule(Form::MIN_LENGTH, 'Heslo musí mít alespoň 5 znaků', 5);

        $passwordConfirmation = $this->addPassword(self::PASSWORD_CONFIRMATION)
            ->setHtmlAttribute('placeholder', 'Znovu heslo')
            ->setRequired();
        $passwordConfirmation->addRule(Form::EQUAL, 'Hesla se musejí shodovat.', $password);
    }

    public function getValues($returnType = null): RegistrationData
    {
        $values = parent::getValues($returnType);

        return new RegistrationData(
            $values[self::FIRST_NAME],
            $values[self::LAST_NAME],
            $values[self::EMAIL],
            (string) $values[self::PHONE],
            $values[self::PASSWORD]
        );
    }
}
