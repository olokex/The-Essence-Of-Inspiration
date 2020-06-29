<?php declare(strict_types = 1);

namespace App\Module\Content\Factory;

use App\Module\Content\Entity\Content;
use App\Module\Content\Repository\EventRepository;
use App\Module\Core\Datagrid\DataGridFactory;
use App\Module\Core\Security\User;
use App\Module\User\Service\AccessGuard;
use Doctrine\ORM\QueryBuilder;
use Ublaboo\DataGrid\DataGrid;
use Ublaboo\DataGrid\DataSource\DoctrineDataSource;

/**
 */
class EventListFactory
{
    private const
        DATE_TIME = 'dateTime',
        HALL = 'hall',
        FORMATTED_PRICE = 'formattedPrice';

    /** @var User */
    private $user;

    /** @var DataGridFactory */
    private $dataGridFactory;

    /** @var EventRepository */
    private $eventRepository;

    public function __construct(DataGridFactory $dataGridFactory, EventRepository $eventRepository, User $user)
    {
        $this->dataGridFactory = $dataGridFactory;
        $this->eventRepository = $eventRepository;
        $this->user = $user;
    }

    public function create(Content $content): DataGrid
    {
        $grid = $this->dataGridFactory->create();
        $grid->setPagination(false);
        $grid->setDataSource(new DoctrineDataSource($this->getQueryBuilder($content), 'e.id'));
        $grid->addColumnDateTime(self::DATE_TIME, 'Datum a čas')
            ->setFormat('j.n.Y H:i');
        $grid->addColumnText(self::HALL, 'Sál', 'hall.title');
        $grid->addColumnText(self::FORMATTED_PRICE, 'Cena');

        $grid->addAction('reserveEvent!', 'Rezervovat');
        $this->conditionallyAppendEditAction($grid);

        return $grid;
    }

    private function conditionallyAppendEditAction(DataGrid $grid): void
    {
        if ($this->user->isLoggedIn() && AccessGuard::hasEditorAccess($this->user->getEntity())) {
            $grid->addAction('editEvent!', 'Upravit');
        }
    }

    private function getQueryBuilder(Content $content): QueryBuilder
    {
        $qb = $this->eventRepository->getQueryBuilder();
        $qb->where('e.content = :content')
            ->setParameter('content', $content);

        return $qb;
    }
}
