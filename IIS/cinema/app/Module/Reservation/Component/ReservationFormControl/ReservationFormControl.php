<?php declare(strict_types = 1);

namespace App\Module\Reservation\Component\ReservationFormControl;

use App\Module\Content\Entity\Event;
use App\Module\Core\Control\AbstractControl;
use App\Module\Core\Form\FormFactory;
use App\Module\Core\Latte\TRenderable;
use App\Module\Reservation\Container\ReservationContactInfoContainer\IReservationContactInfoContainerFactory;
use App\Module\Reservation\Container\ReservationSeatContainer\IReservationSeatContainerFactory;
use App\Module\Reservation\Container\ReservationSeatContainer\ReservationSeatData;
use App\Module\Reservation\Entity\Reservation;
use App\Module\Reservation\Provider\ReservationSeatProvider\ReservationSeatProvider;
use App\Module\Reservation\Service\ReservationService;
use App\Module\User\Entity\User as UserEntity;
use Nette\Application\UI\Form;
use Nette\Application\UI\ITemplate;
use Tracy\Debugger;

/**
 * @method onSuccess(Reservation|null $reservation)
 */
class ReservationFormControl extends AbstractControl
{
    use TRenderable;

    private const
        CONTAINER_CONTACT_INFO = 'contactInfo',
        CONTAINER_SEATS = 'seats';

    /** @var callable[] */
    public $onSuccess = [];

    /** @var FormFactory */
    private $formFactory;

    /** @var Reservation|null */
    private $reservation;

    /** @var Event */
    private $event;

    /** @var IReservationContactInfoContainerFactory */
    private $contactInfoContainerFactory;

    /** @var IReservationSeatContainerFactory */
    private $seatContainerFactory;

    /** @var ReservationSeatProvider */
    private $reservationSeatProvider;

    /** @var ReservationService */
    private $reservationService;

    /** @var UserEntity|null */
    private $userEntity;

    public function __construct(
        Event $event,
        IReservationContactInfoContainerFactory $contactInfoContainerFactory,
        IReservationSeatContainerFactory $seatContainerFactory,
        ReservationSeatProvider $reservationSeatProvider,
        FormFactory $formFactory,
        ReservationService $reservationService,
        ?UserEntity $userEntity = null
    ) {
        $this->formFactory = $formFactory;
        $this->event = $event;
        $this->contactInfoContainerFactory = $contactInfoContainerFactory;
        $this->seatContainerFactory = $seatContainerFactory;
        $this->reservationSeatProvider = $reservationSeatProvider;
        $this->reservationService = $reservationService;
        $this->userEntity = $userEntity;
    }

    protected function setDefaultVariables(ITemplate $template): void
    {
        parent::setDefaultVariables($template);

        $maxSeatInRow = $this->reservationSeatProvider->getMaxSeatInRow($this->event->getHall());
        $template->seatInRowRange = $maxSeatInRow > 0 ? \range(1, $maxSeatInRow) : [];
        $template->seats = $this->reservationSeatProvider->getAllHallSeatsForTemplate($this->event->getHall());
    }

    protected function createComponentForm(): Form
    {
        $form = $this->formFactory->create();
        $form->addComponent($this->contactInfoContainerFactory->create($this->userEntity), self::CONTAINER_CONTACT_INFO);
        $form->addComponent($this->seatContainerFactory->create($this->event), self::CONTAINER_SEATS);
        $form->addSubmit('send', $this->reservation ? 'Upravit' : 'Vytvořit')
            ->setHtmlAttribute('id', 'commit-button');
        $form->onValidate[] = [$this, 'validate'];
        $form->onSuccess[] = [$this, 'process'];

        return $form;
    }

    public function validate(Form $form): void
    {
        /** @var ReservationSeatData $seatData */
        $seatData = $form[self::CONTAINER_SEATS]->getValues();
        $seatCount = \count($seatData->getSeats());

        if (0 === $seatCount) {
            $form->addError('Nevybrali jste žádné sedadlo.');
        }

        if ($seatCount > 5) {
            $form->addError('Na jednu rezervaci lze rezervovat max 5 sedadel.');
        }
    }

    public function process(Form $form): void
    {
        $contactInfoData = $form[self::CONTAINER_CONTACT_INFO]->getValues();
        $seatData = $form[self::CONTAINER_SEATS]->getValues();
        $reservation = $this->reservationService->saveNewReservation($this->event, $contactInfoData, $seatData);

        $this->onSuccess($reservation);
    }
}
