<?php declare(strict_types = 1);

namespace App\Module\User\Factory;

use App\Module\Core\Datagrid\DataGridFactory;
use App\Module\User\Repository\UserRepository;
use Ublaboo\DataGrid\DataGrid;
use Ublaboo\DataGrid\DataSource\DoctrineDataSource;

/**
 */
class UserListFactory
{
    public const
        ROLE = 'role',
        FIRST_NAME = 'firstName',
        LAST_NAME = 'lastName',
        EMAIL = 'email',
        PHONE = 'phone';

    /** @var DataGridFactory */
    private $dataGridFactory;

    /** @var UserRepository */
    private $userRepository;

    public function __construct(DataGridFactory $dataGridFactory, UserRepository $userRepository)
    {
        $this->dataGridFactory = $dataGridFactory;
        $this->userRepository = $userRepository;
    }

    public function create(): DataGrid
    {
        $grid = $this->dataGridFactory->create();
        $grid->setPagination(false);
        $grid->setDataSource(new DoctrineDataSource($this->userRepository->getQueryBuilder(), 'u.id'));
        $grid->addColumnText(self::ROLE, 'Role','role.title');
        $grid->addColumnText(self::FIRST_NAME, 'Jméno');
        $grid->addColumnText(self::LAST_NAME, 'Příjmení');
        $grid->addColumnText(self::EMAIL, 'E-mail');
        $grid->addColumnText(self::PHONE, 'Telefon');

        $grid->addAction('edit!', 'Upravit');
        $grid->addAction('remove!', 'Smazat');

        return $grid;
    }
}
